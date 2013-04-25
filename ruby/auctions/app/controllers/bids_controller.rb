class BidsController < ApplicationController
  before_filter :signed_in

  def create
    @bid = current_user.bids.build(params[:bid])

    if @bid.save
      flash[:success] = 'Your bid has been placed successfully.'
    else
      flash[:error] = "Cannot submit: #{@bid.errors.full_messages.first}"
    end

    redirect_to @bid.auction
  end

  def signed_in
    unless signed_in?
      flash[:notice] = 'Only signed in users have access to this site.'
      redirect_to signin_path
    end
  end
  
end
